import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import protobuf from 'protobufjs'

function App() {
  const [value, setValue] = useState(50);

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
