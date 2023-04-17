import { useState, useEffect } from 'react'
import './App.css'
import { Message } from './bindings/Message';

function App() {
    const [value, setValue] = useState(50);

    const handleValueChange = (event: any) => {
        setValue(event.target.value as any);
        const value = Number(event.target.value);
        const message: Message = {
            target: "Gain",
            message: {
                FloatUpdate: value
            }
        };
        (window as any).external.invoke(JSON.stringify(message));
    };

    return (
      <div className="App">
          <div>
              <input
                  type={"range"}
                  min={0}
                  max={1}
                  step={0.01}
                  value={value}
                  className={"slider"}
                  onChange={handleValueChange}
              />
          </div>
      </div>
    )
}

export default App
