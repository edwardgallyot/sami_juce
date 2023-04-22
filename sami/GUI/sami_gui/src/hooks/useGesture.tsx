import { Message } from '../bindings/Message';
import { useCallback, useState } from "react";
import { Target } from '../bindings/Target'

export const useSetPerformingGesture = (target: Target) => {
    const [performingGesture, setPerformingGesture] = useState(false);

    const sendGesture = () =>{
        const message: Message = {
            target: target,
            message: {
                GestureUpdate: performingGesture,
            }
        };
        (window as any).external.invoke(JSON.stringify(message));
    };

    useCallback(sendGesture, [performingGesture]);

    return [setPerformingGesture];
}
