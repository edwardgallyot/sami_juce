import '../styles/app.css'
import { usePluginInit } from '../hooks/usePluginInit';
import { BasicRangeInput } from './tests/BasicRangeInput';
import { BasicSwitch } from './tests/BasicSwitch';

export const Sami = () => {
    usePluginInit();

    return (
      <section className="mainContent">
        <BasicRangeInput target='Gain'/>
        <BasicRangeInput target='Sustain'/>
        <BasicSwitch target='Bypass'/>
      </section>
    )
}
