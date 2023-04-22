// Plugin Messages
// ============================
// Use the observer pattern to subscribe and unsubscribe for updates from the plug-in on various UI
// elements.
// There shouldn't really be a need to interact with this code directly since we'll wrap it up in nice
// typescript.
function onPluginMessage(){
    this.handlers = new Map();
}

onPluginMessage.prototype = {
    subscribe: function (target, handler)  {
        // If we're the first one we need to create an empty array
        if (!this.handlers.has(target)) {
            this.handlers.set(target, []);
        } 
        let array = this.handlers.get(target);
        array.push(handler);
    },

    unsubscribe: function (target, handler)  {
        if (this.handlers.has(target)) {
            const handlers = this.handlers.get(target);
            // Remove the handler from the map but don't delete the target
            if (typeof(handlers) != "undefined") {
                let index = handlers.indexOf(handler);
                if (index > -1) {
                    handlers.splice(index, 1);
                }
            }
        }
    },

    handle: function (msg) {
        const handlers = this.handlers.get(msg.target);
        if (typeof(handlers) != "undefined") {
            for (const handler of handlers) {
                handler(msg);
            }
        }
    }
}

// Plugin Init
// ============================
// Register and send a unique initalisation message with the webview
function pluginInit(handler) {
    handler();
};

// Globals
// ============================
// These guys sit in the "window" js variable for our typescript wrappers to use

// Give ourselves some global comms objects
window.onPluginMessage = new onPluginMessage();
window.pluginInit = pluginInit;
