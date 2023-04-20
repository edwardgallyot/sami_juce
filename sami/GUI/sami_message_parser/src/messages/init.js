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
        this.handlers.set(target, handler);
    },

    unsubscribe: function (target)  {
        this.handlers.delete(target)
    },

    handle: function (msg) {
        const handler = this.handlers.get(msg.target);
        if (typeof(handler) != "undefined") {
            handler(msg);
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
