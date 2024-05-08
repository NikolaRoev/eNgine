import "../assets/styles.css";
import App from "./App";
import React from "react";
import ReactDOM from "react-dom/client";
import init from "../../../engine/pkg/engine";


async function run() {
    await init();
}

// eslint-disable-next-line no-void
void run();

const root = document.getElementById("root") as HTMLDivElement;

ReactDOM.createRoot(root).render(
    <React.StrictMode>
        <App />
    </React.StrictMode>
);
