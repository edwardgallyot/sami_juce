import { useState, useEffect } from 'react'
import './App.css'

function App() {
    const [value, setValue] = useState(50);
    const [root, setRoot] = useState(new protobuf.Root);

    const handleValueChange = (event: any) => {
        setValue(event.target.value as any);
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
