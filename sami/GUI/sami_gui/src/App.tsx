import { useState, useEffect } from 'react'
import './App.css'
import protobuf from 'protobufjs'

function App() {
    const [value, setValue] = useState(50);
    const loadProtobuf = () => { protobuf.load("messages.proto", (err, root) => {
            if (err) 
                throw err;
            console.log(root);
        });
    };
    useEffect(() => {
        loadProtobuf();
        return () => {};
    }, [])
    return (
      <div className="App">
          <div>
              <input
                  type={"range"}
                  min={1}
                  max={100}
                  value={value}
                  className={"slider"}
                  onChange={(event)=> {setValue(event.target.value as any)}}
              />
          </div>
      </div>
    )
}

export default App
