import { useContext } from "react";
import { DataContext } from "../contexts/data-context";
import { useScenes } from "../hooks/scenes-hooks";
import { set_sprite_position, set_sprite_scale } from "../../../engine/pkg/core/engine";

interface PropertiesProps extends React.ComponentPropsWithoutRef<"div"> {
    test?: number
}



export default function Properties(props: PropertiesProps) {
    const data = useContext(DataContext);
    const scenes = useScenes();

    const sprite = scenes.scenes.get(data?.selectedProperty?.sceneUuid)?.sprites.get(data?.selectedProperty?.objectUuid);

    return (<>{data?.selectedProperty && <div {...props}>
        <div className="flex flex-col">
            <span>SIZE_SCALE_X:</span>
            <input type="number" value={sprite?.scale[0]} onChange={(e) => { set_sprite_scale(data?.selectedProperty?.sceneUuid, data?.selectedProperty?.objectUuid, parseFloat(e.target.value), 0); }}/>
            <span>SIZE_SCALE_Y:</span>
            <input type="number" value={sprite?.scale[1]} onChange={(e) => { set_sprite_scale(data?.selectedProperty?.sceneUuid, data?.selectedProperty?.objectUuid, 0, parseFloat(e.target.value)); }}/>

            <span>X:</span>
            <input type="number" step={10} value={sprite?.transform.position[0]} onChange={(e) => { set_sprite_position(data?.selectedProperty?.sceneUuid, data?.selectedProperty?.objectUuid, parseFloat(e.target.value), sprite?.transform.position[1], sprite?.transform.position[2]); scenes.getScenes(); }}/>
            <span>Y:</span>
            <input type="number" step={10} value={sprite?.transform.position[1]} onChange={(e) => { set_sprite_position(data?.selectedProperty?.sceneUuid, data?.selectedProperty?.objectUuid, sprite?.transform.position[0], parseFloat(e.target.value), sprite?.transform.position[2]); scenes.getScenes(); }}/>
            <span>Z:</span>
            <input type="number" value={sprite?.transform.position[2]} onChange={(e) => { set_sprite_position(data?.selectedProperty?.sceneUuid, data?.selectedProperty?.objectUuid, sprite?.transform.position[0], sprite?.transform.position[1], parseFloat(e.target.value)); scenes.getScenes(); }}/>

            <span>Rotation:</span>
            <input type="number" value={sprite?.transform.rotation} onChange={(e) => { }}/>
        </div>
    </div>}</>);
}
