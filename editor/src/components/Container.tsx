import clsx from "clsx";

export default function Container({ children, className, ...props }: React.ComponentPropsWithoutRef<"div">) {
    return (
        <div
            className={clsx(
                "bg-neutral-700 border-2 p-[5px] m-[1px] border-neutral-400 rounded",
                className
            )}
            {...props}
        >{children}</div>
    );
}
