import Chart from "chart.js/auto";
import { useEffect, useState } from "react";
import 'chartjs-adapter-date-fns';

import {enUS} from 'date-fns/locale';

// const ds = [
//   {temp: 35, humidity: 70},
//   {temp: 36, humidity: 70},
//   {temp: 35, humidity: 64},
//   {temp: 32, humidity: 70},
//   {temp: 35, humidity: 70},
//   {temp: 35, humidity: 70},
//   {temp: 35, humidity: 76},
//   {temp: 34, humidity: 70},
//   {temp: 35, humidity: 70},
//   {temp: 35, humidity: 70},
//   {temp: 35, humidity: 70},
// ]

function getTemp(ds){
  if (!ds)
    return [];
  return ds.map((dsi) => dsi.temp);
}

function getHum(ds){
  if (!ds)
    return [];
  return ds.map((dsi) => dsi.humidity);
}

function getLabels(ds) {
  if (!ds)
    return [];
  return ds.map((dsi) => dsi.date);
}

function getWeatherMessages(allMessages) {
  return allMessages.filter((message) => message.messageType != "alarm");
}

async function getHistory() {
  const response = await fetch("https://robo-backend.nesvo.dev/");
  const json = await response.json();
  return json.messages;
}

function ChartComponent() {
  const [els, setEls] = useState();
  const [chart, setChart] = useState();
  const [handler, setHandler] = useState();

  useEffect(() => {
    // console.log(ds);
    if (!handler)
    {
      setHandler(
        setInterval(() => {
          getHistory().then((msgs) => setEls(getWeatherMessages(msgs)));
        }, 2000)
      );
    }
    console.log(els);
    
    var ctx = document.getElementById('myChart').getContext('2d');
    if (!chart)
    {
      setChart(new Chart(ctx, {
        type: 'line',
        data: {
            labels: getLabels(els),
            datasets: [
              {
                label: "Temperature",
                data: getTemp(els),
                borderColor: 'rgb(242, 100, 48)',
                tension: 0.7
              },
              {
                label: "Humidity",
                data: getHum(els),
                borderColor: 'rgb(81, 113, 165)',
                tension: 0.7
              }
          ]
        },
        options: {
            scales: {
                x: {
                  type: 'time',
                  adapters: {
                    date: {
                        locale: enUS
                    }
                  }
                },
                y: {
                    beginAtZero: true
                }
            }
        }
    }));
    }
    else
    {
      chart.data.datasets[0].data = getTemp(els);
      chart.data.datasets[1].data = getHum(els);
      chart.data.labels = getLabels(els);
      chart.update();
    }
  }, [els]); 

  return (
    <div className="row justify-content-center">
      <div className="col-6">
        <canvas className="col-6" id="myChart" width="400" height="400"></canvas>
      </div>
    </div>
  );
}

export default ChartComponent;