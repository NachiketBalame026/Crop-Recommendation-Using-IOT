
# import serial
# import asyncio
# import websockets


# arduino = serial.Serial(port='COM5', baudrate=9600, timeout=1)

# async def send_data(websocket, path):
#     while True:
#         if arduino.in_waiting > 0:
#             data = arduino.readline().decode('utf-8').rstrip()
#             await websocket.send(data)
#             await asyncio.sleep(1) 
#             print(f"Sent: {data}")

# # # Start the WebSocket server
# # start_server = websockets.serve(send_data, "localhost", 5678)
# # Create the WebSocket server
# async def main():
#     start_server = websockets.serve(send_data, "localhost", 5678)
#     await start_server  # Starts the server
#     await asyncio.Future()  # Run forever


# # asyncio.get_event_loop().run_until_complete(start_server)
# # print("WebSocket server started, streaming Arduino data...")
# # asyncio.get_event_loop().run_forever()
# if __name__ == "__main__":
#     asyncio.run(main())
# import asyncio
# import websockets
# import serial

# # Initialize serial connection to Arduino
# arduino = serial.Serial(port='COM5', baudrate=9600, timeout=1)

# # Function to send data over WebSocket
# async def send_data(websocket, path):  # Ensure this signature is correct
#     while True:
#         # Read data from Arduino
#         if arduino.in_waiting > 0:  # Check if data is available
#             data = arduino.readline().decode('utf-8').strip()  # Read a line of data
#             print(f"Sending data: {data}")  # Optional: print the data being sent
#             await websocket.send(data)  # Send the data to WebSocket client
#         await asyncio.sleep(1)  # Wait before checking again

# # Create the WebSocket server
# async def main():
#     start_server = websockets.serve(send_data, "localhost", 5678)
#     await start_server  # Starts the server
#     await asyncio.Future()  # Run forever

# # Start the event loop
# if __name__ == "__main__":
#     asyncio.run(main())
import serial
import asyncio
import websockets

# Configure the serial port (replace 'COM3' with the appropriate port for Arduino)
serial_port = 'COM5'  # Replace with your port
baud_rate = 9600
arduino = serial.Serial(serial_port, baud_rate, timeout=1)

# WebSocket server handler
async def send_sensor_data(websocket):
    try:
        while True:
            # Read data from Arduino
            if arduino.in_waiting > 0:
                data = arduino.readline().decode('utf-8').strip()
                print(f"Sending data: {data}")
                
                # Send data over WebSocket to connected clients
                await websocket.send(data)
                await asyncio.sleep(0.1)  # Small delay to prevent overwhelming
    except websockets.exceptions.ConnectionClosed:
        print("Client disconnected.")

# Start the WebSocket server
async def main():
    async with websockets.serve(send_sensor_data, "localhost", 8765):
        print("WebSocket server started on ws://localhost:8765")
        await asyncio.Future()  # Run forever

# Run the server
asyncio.run(main())