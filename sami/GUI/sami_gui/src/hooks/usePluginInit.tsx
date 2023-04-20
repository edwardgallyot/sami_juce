import { useEffect } from 'react'
import { Message } from '../bindings/Message';
import { MessageType } from '../bindings/MessageType';

export const usePluginInit = () => {

    const initHandler = () => {
        const init: MessageType = "Init";
        const message: Message = {
            target: null,
            message: init,
        };
        console.log(message);
        (window as any).external.invoke(JSON.stringify(message));
    };

    useEffect(() => {
        (window as any).pluginInit(initHandler);
    }, []);
}
