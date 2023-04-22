import { useBoolParam } from "../../hooks/useBoolParam";
import { useSetPerformingGesture } from "../../hooks/useGesture";
import { SwitchProps } from "../../props/switchProps";

export const BasicSwitch = (props: SwitchProps) => {
    const {target} = props;
    const [value, setValue] = useBoolParam(props.target);
    const [setPerformingGesture] = useSetPerformingGesture(target);
    const handleClick = () => {
        setPerformingGesture(true);
        setValue(!value);
        setPerformingGesture(false);
    };
    return (
        <button 
            onClick={handleClick}
        >
            {value ? "Bypassed" : "Not Bypassed"}
        </button>
    );
}

