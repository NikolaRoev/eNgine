import { type Scenes, get_scenes } from "../../../engine/pkg/core/engine";
import { useEffect, useState } from "react";



export function useScenes() {
    const [scenes, setScenes] = useState<Scenes>(new Map());

    function getScenes() {
        setScenes(get_scenes());
    }

    useEffect(() => {
        getScenes();
    }, []);

    return { scenes, getScenes };
}
