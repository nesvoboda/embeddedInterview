from typing import Optional, List

from fastapi import FastAPI

app = FastAPI()

from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware

origins = [
    "http://localhost",
    "http://localhost:3000",
    "http://nesvo-bot.netlify.app",
    "https://nesvo-bot.netlify.app",
    "https://bot.nesvo.dev",
    "http://bot.nesvo.dev",
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class Weather(BaseModel):
    temp: float
    humidity: float

class Message(BaseModel):
    messageType: str
    temp: Optional[float] = None
    humidity: Optional[float] = None
    date: str

messages: List[Message] = []
sentryMode: bool = True

class Resp(BaseModel):
    messages: List[Message]

@app.post("/message/")
async def handle_message(message: Message):
    messages.append(message)
    if len(messages) > 30:
        messages.pop(0)

@app.get("/last/")
async def handle_last():
    return messages[-1]

@app.get("/sentry")
async def get_sentry():
    return sentryMode

@app.post("/sentry")
async def set_sentry():
    global sentryMode
    if sentryMode is True:
        sentryMode = False
    else:
        sentryMode = True
    return sentryMode

@app.get("/")
async def handle_hello():
    return {"messages": messages}