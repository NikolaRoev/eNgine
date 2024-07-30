interface ConsoleProps extends React.ComponentPropsWithoutRef<"div"> {
    test?: number
}


export default function Console(props: ConsoleProps) {
    return <div {...props}>console</div>;
}
