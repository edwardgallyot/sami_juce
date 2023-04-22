import '../../styles/app.css'
import { useFloatParam } from '../../hooks/useFloatParam';
import { RangeProps } from '../../props/rangeProps';
import { useSetPerformingGesture } from '../../hooks/useGesture';

export const BasicRangeInput = (props: RangeProps) =>  {
    const [value, setValue] = useFloatParam(props.target);
    const [setPerforimingGesture] = useSetPerformingGesture(props.target);
    const handleChange = (e: any) => {
        setValue(Number(e.target.value)); 
    };
    return  (
        <div className='basicRangeInput'>
          <input
              id='BasicRangeInput'
              className='input'
              type={"range"}
              min={0}
              max={1}
              step={0.00001}
              value={value}
              onMouseDown={() => { setPerforimingGesture(true); }}
              onMouseUp={() => { setPerforimingGesture(false); }}
              onChange={handleChange}
          />
       </div>
    );
};
