# mostly LLM generated, modified to actually work
# pip install pyserial influxdb-client

import serial
import threading
import re
import sys
import os
import time

from influxdb_client import InfluxDBClient, Point, WriteOptions, WritePrecision

# Import config
import BSG2Influx_config as config


# ==============================
# INFLUX SETUP (BATCHED)
# ==============================

client = InfluxDBClient(
    url=config.INFLUX_URL,
    token=config.INFLUX_TOKEN,
    org=config.INFLUX_ORG
)

write_api = client.write_api(
    write_options=WriteOptions(
        batch_size=1000,
        flush_interval=1000,
        jitter_interval=0,
        retry_interval=5000
    )
)

# ==============================
# FAST VALUE PARSER
# ==============================

number_extract = re.compile(r"-?\d+\.?\d*")

def parse_telegram(lines):
    data = {}

    for line in lines:
        if "]" in line:
            line = line.split("]", 1)[1]

        parts = line.split(",")

        for part in parts:
            if ":" not in part:
                continue

            key, value = part.split(":", 1)
            key = key.strip()
            value = value.strip()

            match = number_extract.search(value)
            if not match:
                continue

            num = match.group()

            if "." in num:
                data[key] = float(num)
            else:
                data[key] = int(num)

    return data


# ==============================
# SERIAL DEVICE THREAD
# ==============================

class SerialDevice(threading.Thread):
    def __init__(self, port, name):
        super().__init__(daemon=True)
        self.name = name
        self.ser = serial.Serial(port, config.BAUDRATE, timeout=1)
        self.running = True
        self.buffer = []
        self.do_print_request = False

    def run(self):
        print(f"[INFO] Started {self.name}")

        while self.running:
            try:
                line = self.ser.readline().decode(errors="ignore").strip()
                if not line:
                    continue
                if line.startswith("Mode:"):
                    continue

                if line.startswith("[01E]"):
                    if self.buffer:
                        self.process_telegram(self.buffer)
                        self.buffer = []

                self.buffer.append(line)

                if self.do_print_request and line.startswith("[01F]"):
                    print(line)
                    self.do_print_request = False

            except Exception as e:
                print(f"[ERROR] {self.name}: {e}")

    def process_telegram(self, lines):
        parsed = parse_telegram(lines)
        if not parsed:
            return

        point = Point(config.MEASUREMENT_NAME).tag("device", self.name).time(time.time_ns(), WritePrecision.NS)

        for k, v in parsed.items():
            point.field(k, v)

        # print(point)
        write_api.write(bucket=config.INFLUX_BUCKET, record=point)
        # print(time.time())

    def print_request(self):
        self.do_print_request = True

    def send_command(self, char):
        self.ser.write(char.encode())

    def stop(self):
        self.running = False
        self.ser.close()


def get_single_key():
    """
    Reads a single keypress from stdin without requiring Enter.
    Works on Windows and Linux.
    """
    if os.name == "nt":
        import msvcrt
        return msvcrt.getch().decode(errors="ignore")
    else:
        import tty
        import termios

        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)

        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

        return ch

# ==============================
# MAIN
# ==============================

def main():
    devices = []
    selected_index = 0

    # Start serial devices
    for cfg in config.SERIAL_PORTS:
        dev = SerialDevice(cfg["port"], cfg["name"])
        dev.start()
        devices.append(dev)

    print("\n=== Control via stdin ===")
    print("Enter device number to select it (1,2,3...)")
    print("Enter single character to send command")
    print("Type 'exit' to quit\n")

    print(f"Selected: 1 ({devices[0].name})")

    while True:
        try:
            user_input = get_single_key()
        except KeyboardInterrupt:
            break

        if not user_input:
            continue

        if user_input == '\x03':
            break

        # Device selection
        if user_input.isdigit():
            idx = int(user_input) - 1
            if 0 <= idx < len(devices):
                selected_index = idx
                print(f"Selected: {user_input} ({devices[idx].name})")
            else:
                print("Invalid device number")
            continue

        # Send single-character command
        if (user_input == ' '):
            for dev in devices: dev.send_command(user_input)
            print(f"Sent '{user_input}' to all devices.")
        elif len(user_input) == 1:
            devices[selected_index].send_command(user_input)
            print(f"{user_input} > {devices[selected_index].name} \t", end='')
            time.sleep(0.01)
            devices[selected_index].print_request()
        else:
            print("Enter either a device number or a single character command")



    # Cleanup
    for d in devices:
        d.stop()

    client.close()


if __name__ == "__main__":
    main()