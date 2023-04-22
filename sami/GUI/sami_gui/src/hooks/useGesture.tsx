import { Message } from '../bindings/Message';
import { useState } from "react";
import { Target } from '../bindings/Target'

export const useGesture = (target: Target) => {
    const [performingGesture, setPerformingGesture] = useState(true);
    const handleGesture = (gestureIsStarting: boolean) => {
        if (performingGesture != gestureIsStarting) {
            setPerformingGesture(gestureIsStarting);
            const message: Message = {
                target: target,
                message: {
                    GestureUpdate: gestureIsStarting,
                }
            };
            (window as any).external.invoke(JSON.stringify(message));
        }
    };
    return [handleGesture];
}
