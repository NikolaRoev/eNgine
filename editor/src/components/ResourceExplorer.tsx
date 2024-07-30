interface ResourceExplorerProps extends React.ComponentPropsWithoutRef<"div"> {
    test?: number
}


export default function ResourceExplorer(props: ResourceExplorerProps) {
    return <div {...props}>resource explorer</div>;
}
