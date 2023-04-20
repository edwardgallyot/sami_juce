import './App.css'
import { usePluginInit } from './hooks/usePluginInit';
import { useFloatParam } from './hooks/useFloatParam';

function App() {
    usePluginInit();
    const [value, handleValueChange] = useFloatParam("Gain");

    return (
      <div className="App">
          <div>
              <input
                  type={"range"}
                  min={0}
                  max={1}
                  step={0.01}
                  value={value}
                  onChange={(e) => {handleValueChange(Number(e.target.value))}}
              />
          </div>
      </div>
    )
}

export default App
