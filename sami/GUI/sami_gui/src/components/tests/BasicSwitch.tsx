import { useBoolParam } from "../../hooks/useBoolParam";
import { SwitchProps } from "../../props/switchProps";

export const BasicSwitch = (props: SwitchProps) => {
    const [value, setValue] = useBoolParam(props.target);
    const handleClick = () => {
        setValue(!value);
    };
    return (
        <button 
            onClick={handleClick}
        >
            {value ? "Bypassed" : "Not Bypassed"}
        </button>
    );
}

