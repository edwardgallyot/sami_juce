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

window.onPluginMessage = new onPluginMessage();
