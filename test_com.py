import serial
import time

# Configure the serial port
ser = serial.Serial(port='COM20', baudrate=9600, timeout=1)

# Give the port some time to initialize
time.sleep(2)

message = "Hello from Python!\n"

print("Sending message every 100 ms. Press Ctrl+C to stop.")
try:
    while True:
        ser.write(message.encode('utf-8'))
        print(f"Sent: {message.strip()}")
        time.sleep(0.1)  # 100 milliseconds
except KeyboardInterrupt:
    print("\nStopped by user.")
finally:
    ser.close()
