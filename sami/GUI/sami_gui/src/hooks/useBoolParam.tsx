import { Target } from "../bindings/Target";
import { MessageType } from '../bindings/MessageType';
import { Message } from '../bindings/Message';
import { useState, useEffect, useCallback } from 'react';


export const useBoolParam = (target: Target) => {

    const [value, setValue] = useState(false);

    const pluginHandler = (msg: Message) => {
        const update: MessageType = msg.message as {BoolUpdate: boolean;};
        if (typeof(update) != "undefined") {
            setValue(update.BoolUpdate);
        }
    };

    const handleValueEvent = (value: boolean) => {
        const message: Message = {
            target: target,
            message: {
                BoolUpdate: value
            }
        };
        (window as any).external.invoke(JSON.stringify(message));
    };

    const valueSetter = useCallback(handleValueEvent, [value]);

    useEffect(() => {
        (window as any).onPluginMessage.subscribe(target, pluginHandler);
        return () => {
            (window as any).onPluginMessage.unsubscribe(target, pluginHandler);
        };
    }, []);

    return [value, valueSetter] as const;
};
