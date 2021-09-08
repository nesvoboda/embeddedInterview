import { useEffect, useState } from "react";


async function getMessage() {
  const response = await fetch("https://robo-backend.nesvo.dev/last/");
  const json = await response.json();
  return json;
}

function Data() {
  const [weather, setWeather] = useState();
  const [message, setMessage] = useState();
  const [temp, setTemp] = useState();
  const [humidity, setHumidity] = useState();
  const [alarm, setAlarm] = useState(false);
  const [handler, setHandler] = useState();

  useEffect(async () => {
    if (!handler) {
      setHandler(
        setInterval(() => {
          getMessage().then((message) => {
            setMessage(message);
            setWeather((oldW) => oldW ? [...oldW, message] : [message]);
          });
        }, 500)
      );
    }
    if (message) {
      if (message.messageType === "alarm") {
        setAlarm(true);
      } else {
        setAlarm(false);
        setTemp(message.temp);
        setHumidity(message.humidity);
        setWeather((oldW) => oldW ? [...oldW, message] : [message]);
      }
    }
  }, [message]);

  return (
    <div>
      {alarm ? <p>Alarm</p> : null}
        <p>Temp: {temp}</p>
        <p>Humidity: {humidity}</p>
    </div>
  );
}
export default Data;