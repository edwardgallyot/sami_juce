import { Message } from '../bindings/Message';
import { Target } from '../bindings/Target'
import { MessageType } from '../bindings/MessageType';
import { useState, useEffect, useCallback } from 'react';

export const useFloatParam = (target: Target) => {
    const [value, setValue] = useState(0);

    const pluginHandler = (msg: Message) => {
        const update: MessageType = msg.message as {FloatUpdate: number;};
        if (typeof(update) != "undefined") {
            setValue(update.FloatUpdate);
        }
    };

    const handleValueEvent = (value: number) => {
        const message: Message = {
            target: target,
            message: {
                FloatUpdate: value
            }
        };
        (window as any).external.invoke(JSON.stringify(message));
    };

    useCallback(handleValueEvent, [value]);

    useEffect(() => {
        (window as any).onPluginMessage.subscribe(target, pluginHandler);
        return () => {
            (window as any).onPluginMessage.unsubscribe(target, pluginHandler);
        };
    }, []);
    return [value, setValue] as const;
}
