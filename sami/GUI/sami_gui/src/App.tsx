import { useState, useEffect } from 'react'
import './App.css'
import { Message } from './bindings/Message';

function App() {
    const [value, setValue] = useState(50);

    const handleValueChange = (event: any) => {
        setValue(event.target.value as any);
        const value = Number(event.target.value);
        const message: Message = {
            id: "Update",
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
                  min={1}
                  max={100}
                  value={value}
                  className={"slider"}
                  onChange={handleValueChange}
              />
          </div>
      </div>
    )
}

export default App
