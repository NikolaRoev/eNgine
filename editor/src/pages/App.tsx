import { useEffect, useState } from "react";
// eslint-disable-next-line camelcase
import init, { move_rect, move_cam } from "../../../engine/pkg/engine";




interface TransformControlsProps {
    x: number,
    y: number,
    setX: (newX: number) => void,
    setY: (newY: number) => void,
}
function TransformControls(props: TransformControlsProps) {
    return (
        <div>
            <div>
                <span>X:</span>
                <input type="number" value={props.x} onChange={(e) => { props.setX(parseFloat(e.target.value)); }}/>
                <span>Y:</span>
                <input type="number" value={props.y} onChange={(e) => { props.setY(parseFloat(e.target.value)); }}/>
            </div>
            <button onClick={() => { props.setX(props.x + 10); }}>{">"}</button>
            <button onClick={() => { props.setX(props.x - 10); }}>{"<"}</button>
            <button onClick={() => { props.setY(props.y + 10); }}>{"^"}</button>
            <button onClick={() => { props.setY(props.y - 10); }}>{"v"}</button>
        </div>
    );
}


function Test() {
    const [state, setState] = useState({x: 0, y: 0, cam_x: 0, cam_y: 0});

    useEffect(() => {
        move_rect(state.x, state.y);
        move_cam(state.cam_x, state.cam_y);
    }, [state]);

    return (
        <>
            <input type="range" min="0" max="1024" value={state.x} onChange={(e) => { setState({...state, x: parseFloat(e.target.value)}); }}/>
            <input type="range" min="0" max="576" value={state.y} onChange={(e) => { setState({...state, y: parseFloat(e.target.value)}); }}/>
            <TransformControls
                x={state.cam_x}
                y={state.cam_y}
                setX={(newX) => { setState({...state, cam_x: newX}); }}
                setY={(newY) => { setState({...state, cam_y: newY}); }}
            />
        </>
    );
}


export default function App() {
    const [loaded, setLoaded] = useState(false);

    useEffect(() => {
        init().then(() => { setLoaded(true); }).catch(() => { console.log("ey"); });
    });

    return (
        <div className="grow flex flex-col">
            {loaded && <Test />}
            <canvas width={1280} height={576} id="canvas" className="grow"></canvas>
        </div>
    );
}
