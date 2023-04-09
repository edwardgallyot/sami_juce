import { useState, useEffect } from 'react'
import './App.css'
import protobuf, { Root } from 'protobufjs'
import { TextDecoder } from 'text-decoding'

function App() {
    const [value, setValue] = useState(50);
    const [root, setRoot] = useState(new protobuf.Root);


    const loadProtobuf = () => { protobuf.load("./messages.proto", root, (err, root) => {
            if (err) 
                throw err;
            setRoot(root as Root);
        });
    };

    useEffect(() => {
        loadProtobuf();
        return () => {};
    }, [])

    const handleValueChange = (event: any) => {
        setValue(event.target.value as any);
        var test = root.lookupType("sami.Test");
        var payload = {
            message: event.target.value 
        };
        var err = test.verify(payload);
        console.log(err);
        var target = test.create(payload);
        const buffer = test.encode(target).finish();
        const stringpayload = new TextDecoder().decode(buffer);
        console.log(stringpayload);
        (window as any).external.invoke(stringpayload);
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
