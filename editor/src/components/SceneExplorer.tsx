import { useContext, useEffect } from "react";
import { DataContext } from "../contexts/data-context";
import { useScenes } from "../hooks/scenes-hooks";
import Container from "./Container";

interface SceneExplorerProps extends React.ComponentPropsWithoutRef<"div"> {
    test?: number
}


export default function SceneExplorer(props: SceneExplorerProps) {
    const scenes = useScenes();
    const data = useContext(DataContext);

    //scenes.getScenes();

    return (
        <Container {...props}>
            {Array.from(scenes.scenes.values()).map((scene) => {
                const sprites = Array.from(scene.sprites.values());

                return (
                    <div key={scene.id.uuid}>
                        <span>{scene.id.uuid}</span>
                        <span>{scene.id.name}</span>

                        {sprites.map((sprite) => (
                            <div key={sprite.id.uuid}>
                                <span>{sprite.id.uuid}</span>
                                <button
                                    onClick={ () => {

                                        

                                        data?.setSelectedProperty({
                                            sceneUuid: scene.id.uuid,
                                            type: "Sprite",
                                            objectUuid: sprite.id.uuid });
                                    } }
                                >{sprite.id.name}</button>
                            </div>
                        ))}
                    </div>
                );
            })}
        </Container>
    );
}
