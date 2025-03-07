"""Performs websocket connection to frontend.

Responsibilities:
- starts connection
- receives messages and requests from frontend, responds
"""
import json
import asyncio
from websockets.server import serve

HOST = "localhost"
PORT = 8765

def websockets_process():
    async def send_response(message_dict, websocket):
        """Send response message based of inbound message type"""
        if message_dict['type'] == "increment":
            message_to_sent = {
                "type": "increment_acknowledgement",
                "timestamp": message_dict["timestamp"],
            }
            await websocket.send(json.dumps(message_to_sent))

    async def echo(websocket):
        async for message in websocket:
            try:
                message_dict = json.loads(message)
                print(f'Msg received: {message_dict}')
                await send_response(message_dict, websocket)
            except Exception as error:
                print(error)

    async def ws_server():
        async with serve(echo, HOST, PORT, max_size=10 * 1024):
            await asyncio.Future()  # run forever

    async def main():
        result = await asyncio.gather(ws_server())

    asyncio.run(main())

if __name__ == "__main__":
    websockets_process()
