#!/usr/bin/env python3

# Utility for interfacing with CAN XCP.
# Use it for dumping / poking memory locations
# Example usages:
# xcp_util.py dump -s 0x0 -e 0x1FFFFF
# xcp_util.py poke -a 0x4000ecd6 1
#
# requirements: pip install python-can


import argparse
import threading
import struct
import can
import time
import sys
import os

running = True

def can_testerPresent_thread(args, bus: can.Bus):
    print("Entering extended diagnostic session via UDS")
    # Enter extended diagnostic session
    msg = can.Message(arbitration_id=args.udsid, data=[0x02, 0x10, 0x03, 0, 0, 0, 0, 0], is_extended_id=False)
    bus.send(msg)
    time.sleep(0.5)
    bus.send(msg)   # send twice to make sure device is awake

    while running:
        time.sleep(1)
        # Tester present
        msg = can.Message(arbitration_id=args.udsid, data=[0x01, 0x3E, 0, 0, 0, 0, 0, 0], is_extended_id=False)
    pass

def hex_int(x):
    return int(x, 16)

def main():
    parser = argparse.ArgumentParser(description="XCP Util")

    parser.add_argument('--txid', type=hex_int, default=0x18EF6201, help="XCP Transmit Arbitration CAN ID")
    parser.add_argument('--rxid', type=hex_int, default=0x18EF6202, help="XCP Receive Arbitration CAN ID")
    parser.add_argument('--udsid', type=hex_int, default=0x7E6, help="UDS TX CAN ID (needed to enter diagnostic session and keep ECU awake)")
    parser.add_argument('--interface', type=str, default='pcan', help="Interface Name, see: https://python-can.readthedocs.io/en/stable/configuration.html#interface-names")
    parser.add_argument('--channel', type=str, default='PCAN_USBBUS1', help="Interface Channel, see the respective interface documentation")
    parser.add_argument('--bitrate', type=int, default=500000)

    subparsers = parser.add_subparsers(        
        title='commands',
        # description='Available commands',
        # help='Use one of the following commands:',
        dest='command')
    
    cmd_dump = subparsers.add_parser(
        'dump',
        help='Dumps memory region'
    )
    cmd_dump.add_argument('--start', '-s', '-a', type=hex_int, default=0, help='Start address', required=True)
    cmd_dump.add_argument('--end', '-e', type=hex_int, default=0, help='End address, inclusive')
    cmd_dump.add_argument('--length', '-n', type=hex_int, default=4, help='Number of bytes to dump')
    cmd_dump.add_argument('--name', '-O', type=str, default='dump.bin', help='Output filename')
    

    cmd_poke = subparsers.add_parser(
        'poke',
        help='Write bytes to arbitrary location'
    )
    cmd_poke.add_argument('--address', '-a', type=hex_int, help='Adress to write to', required=True)
    cmd_poke.add_argument('data', type=hex_int, help='Data, hex (currently only supports 1/2/4 bytes)')
    cmd_poke.add_argument('len', type=int, default=1, nargs='?', help='Length of data, hex (optional, assumed 1-4 bytes depending on input)')


    global args
    args = parser.parse_args()

    try:

        bus = can.interface.Bus(interface=args.interface, channel=args.channel, bitrate=args.bitrate)
        thr_keepalive = threading.Thread(target=can_testerPresent_thread, args=(args, bus,))
        thr_keepalive.daemon = True
        thr_keepalive.start()

        listener = threading.Thread(target=can_listener, args=(bus,))
        listener.daemon = True
        listener.start()

        time.sleep(1)   # make sure ECU is in diagnostic session

        # Implement the functionality based on subcommand
        if args.command == 'dump':
            dump(args, bus)
        elif args.command == 'poke':
            poke(args, bus)
        else:
            parser.print_help()
    except KeyboardInterrupt: 
        global running
        running = False
        sys.exit(1)
        time.sleep(3)
        os._exit(1)


cb_msg = None
event = threading.Event()

def blocking_cb(msg):
    global cb_msg, event
    if msg.arbitration_id != args.rxid:
        return
    if msg.data[0] == 0xfe:
        print("Error:", msg)
        cb_msg = None
    else:
        cb_msg = msg
    event.set()

def send_blocking(bus, arb_id, data, is_extended = True):
    global cb_msg
    cb_msg = None
    msg = can.Message(arbitration_id=arb_id, data=data, is_extended_id=is_extended)
    bus.send(msg)
    event.wait(5)
    event.clear()
    return cb_msg

def can_listener(bus):
    while True:
        msg = bus.recv()
        if msg:
            blocking_cb(msg)

def dump(args, bus: can.Bus):
    if args.end == 0:    # (not default)
        args.end = args.start + args.length - 1
    
    print(f"Reading from {args.start:08X} to {args.end:08X}.")

    with open(args.name, 'wb') as bin_file:
        send_blocking(bus, args.txid, [0xFF])  # CONNECT

        addr = struct.pack('>L', args.start)
        send_blocking(bus, args.txid, [0xf6, 0x00, 0x00, 0x00] + list(addr))  # SET_MTA (seek)

        for i in range(args.start, args.end + 1, 0x7):
            num_bytes = min(0x07, args.end + 1 - i)
            response = send_blocking(bus, args.txid, [0xf5, num_bytes, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])  # Upload
            if response is None:
                print(f"Timeout or error at address {i:08X}")
                continue

            data = response.data

            # print hex
            for j in range(i, i + num_bytes):
                if j % 16 == 0:
                    print(f'\n{j:08X}: ', end='')
                print(f'{data[1 + j - i]:02X} ', end='')

            # write to file
            bin_file.write(data[1:1+num_bytes])

def poke(args, bus: can.Bus):
    print(send_blocking(bus, args.txid, [0xFF]))  # CONNECT
    addr = struct.pack('>L', args.address)
    print(send_blocking(bus, args.txid, [0xf6, 0x00, 0x00, 0x00] + list(addr)))  # SET_MTA (seek)
    data = struct.pack('>L', args.data)

    # set correct data length
    if args.len == 1:
        if (args.data > 0xFFFF):
            args.len = 4
        elif (args.data > 0xFF):
            args.len = 2
            data = struct.pack('>H', args.data)
        else:
            data = struct.pack('>B', args.data)

    response = send_blocking(bus, args.txid, [0xf0, args.len] + list(data) + [0x00, 0x00])  # Download
    if response is None:
        print(f"Timeout or error")
    print(response)
    


if __name__ == "__main__":
    main()
