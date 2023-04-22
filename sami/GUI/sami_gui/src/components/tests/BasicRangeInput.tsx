import '../../styles/app.css'
import { useFloatParam } from '../../hooks/useFloatParam';
import { RangeProps } from '../../props/rangeProps';
import { useSetPerformingGesture } from '../../hooks/useGesture';

export const BasicRangeInput = (props: RangeProps) =>  {
    const [value, setValue] = useFloatParam(props.target);
    const [setPerforimingGesture] = useSetPerformingGesture(props.target);
    return  (
        <div className='basicRangeInput'>
          <input
              id='BasicRangeInput'
              className='input'
              type={"range"}
              min={0}
              max={1}
              step={0.01}
              value={value}
              onMouseDown={() => { setPerforimingGesture(true); }}
              onMouseUp={() => { setPerforimingGesture(false); }}
              onChange={(e) => { setValue(Number(e.target.value)); }}
          />
       </div>
    );
};
