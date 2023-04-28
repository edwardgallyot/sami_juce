import '../styles/app.css'
import { usePluginInit } from '../hooks/usePluginInit';
import { BasicRangeInput } from './tests/BasicRangeInput';
import { BasicSwitch } from './tests/BasicSwitch';

export const Sami = () => {
    usePluginInit();

    return (
      <section className="mainContent">
        <BasicRangeInput target='Gain'/>
        <BasicRangeInput target='Attack'/>
        <BasicRangeInput target='Decay'/>
        <BasicRangeInput target='Sustain'/>
        <BasicRangeInput target='Release'/>
        <BasicSwitch target='Bypass'/>
      </section>
    )
}
