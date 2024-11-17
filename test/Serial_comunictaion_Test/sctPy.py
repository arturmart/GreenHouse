
#pip3 install pyserial
#sudo python3 sctPy.py

import serial
import time

# Initialize serial communication with the correct port and baud rate
# Replace '/dev/ttyS0' with the appropriate serial port on your Orange Pi
serial_port = '/dev/ttyS3'  # Update this based on your setup
baud_rate = 9600            # Make sure this matches the other device

# Set up the serial connection
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Give some time for the serial connection to initialize
time.sleep(2)

try:
    # Data to send
    data = "Hello, Arduino!"
    
    # Send the data (encode to bytes if it's a string)
    ser.write(data.encode())
    print(f"Data sent: {data}")
    
    # Optional: receive response (if you expect one)
    time.sleep(1)  # Wait for response
    if ser.in_waiting > 0:
        received_data = ser.read(ser.in_waiting).decode()  # Read all available bytes
        print("Received:", received_data)

finally:
    # Close the serial connection when done
    ser.close()