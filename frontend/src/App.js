import logo from "./logo.svg";
import "./App.css";
import { useEffect, useState } from "react";
import ChartComponent from "./chart.component";
import Data from "./data.component";


async function getSentry() {
  const response = await fetch("https://robo-backend.nesvo.dev/sentry");
  const json = await response.json();
  console.log(json);
  return json;
}

async function toggleSentry() {
  const response = await fetch("https://robo-backend.nesvo.dev/sentry",
  {
    method: "POST"
  });
  const json = await response.json();
  console.log(json);
  return json;
}


function App() {
  const [sentry, setSentry] = useState();

  const handleSentry = () => {
    toggleSentry().then((sentry)=>setSentry(sentry));
  }

  useEffect(async () => {
    if (!sentry)
    {
      getSentry().then((sentry) => setSentry(sentry));
    }
  }, []);

  return (
    <div className="App">
      <div class="container">
        <h1>Robot</h1>
        <Data/>
        <p>Sentry: {sentry === true ? "On" : "Off"}</p>
        <button onClick={handleSentry}>Toggle sentry</button>
      </div>
      <ChartComponent />
    </div>
  );
}

export default App;
