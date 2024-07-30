
import { createContext, useState } from "react";



type SelectedProperty = {
    sceneUuid: string,
    type: "Sprite",
    objectUuid: string
}

type DataContextContents = {
    selectedProperty: SelectedProperty | null,
    setSelectedProperty: React.Dispatch<React.SetStateAction<SelectedProperty | null>>
}

export const DataContext = createContext<DataContextContents | null>(null);


export default function DataContextProvider({ children }: { children: React.ReactNode }) {
    const [selectedProperty, setSelectedProperty] = useState<SelectedProperty | null>(null);

    return (
        <DataContext.Provider value={{
            selectedProperty: selectedProperty,
            setSelectedProperty: setSelectedProperty
        }}>{children}</DataContext.Provider>
    );
}
