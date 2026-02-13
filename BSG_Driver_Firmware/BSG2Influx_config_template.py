# ==============================
# SERIAL CONFIGURATION
# ==============================

SERIAL_PORTS = [
    {"port": "COM3", "name": "device_1"},
    {"port": "COM4", "name": "device_2"},
]

BAUDRATE = 115200

# ==============================
# INFLUXDB CONFIGURATION
# ==============================

INFLUX_URL = "http://localhost:8086"
INFLUX_TOKEN = "YOUR_SECRET_TOKEN"
INFLUX_ORG = "your_org"
INFLUX_BUCKET = "your_bucket"

MEASUREMENT_NAME = "motor_data"