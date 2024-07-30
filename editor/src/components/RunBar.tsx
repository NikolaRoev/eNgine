import { BsPause, BsPlay, BsStop } from "react-icons/bs";
import clsx from "clsx";

interface RunBarProps extends React.ComponentPropsWithoutRef<"div"> {
    test?: number
}


export default function RunBar({ className, ...props }: RunBarProps) {
    return (
        <div className={clsx("flex justify-evenly", className)} {...props}>
            <button><BsPlay /></button>
            <button><BsPause /></button>
            <button><BsStop /></button>
        </div>
    );
}
