import { Message } from '../bindings/Message';
import { Target } from '../bindings/Target'
import { MessageType } from '../bindings/MessageType';
import { useState, useEffect } from 'react';

export const useFloatParam = (target: Target) => {

    const [value, setValue] = useState(0);

    const gainHandler = (msg: Message) => {
        const update: MessageType = msg.message as {FloatUpdate: number;};
        console.log(update);
        if (typeof(update) != "undefined") {
            setValue(update.FloatUpdate);
        }
    };

    const handleValueEvent = (event: any) => {
        const value = Number(event.target.value);
        const message: Message = {
            target: target,
            message: {
                FloatUpdate: value
            }
        };
        (window as any).external.invoke(JSON.stringify(message));
        setValue(event.target.value as any);
    };

    useEffect(() => {
        (window as any).onPluginMessage.subscribe(target, gainHandler);
    }, []);

    return [value, handleValueEvent] as const;
}
