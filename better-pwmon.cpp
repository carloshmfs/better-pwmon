#include <pipewire/pipewire.h>

#include <iostream>
#include <format>
#include <string>

struct data {
    struct pw_main_loop* loop;
    struct pw_context* context;
    struct pw_core* core;

    struct pw_registry* registry;
    struct spa_hook registry_listener;

    struct pw_client* client;
    struct spa_hook client_listener;

    struct spa_hook node_listener;
};

static void on_param(void *data, int seq, uint32_t id, uint32_t index, uint32_t next, const struct spa_pod *param) {
    (void)data;
    (void)seq;
    (void)index;
    (void)next;
    (void)param;
    if (id == SPA_PARAM_Props) {
        // struct spa_pod_object *obj = (struct spa_pod_object *)param;
        // parse the pod for volume (e.g., SPA_PROP_channelVolumes, SPA_PROP_volume)
        std::cout << "HERE" << std::endl;
    }
}

static const struct pw_node_events node_events = {
    .version = PW_VERSION_NODE_EVENTS,
    .info = nullptr,
    .param = on_param
};

static void registry_event_global(void *data, uint32_t id, uint32_t permissions, const char *type, uint32_t version, const struct spa_dict *props)
{
    (void)permissions;
    (void)props;

    auto pw_data = static_cast<struct data*>(data);

    if (std::string(type) == PW_TYPE_INTERFACE_Node) {
        auto bind = pw_registry_bind(pw_data->registry, id, PW_TYPE_INTERFACE_Node, version, 0);
        auto node = static_cast<struct pw_node*>(bind);
        pw_node_add_listener(node, &pw_data->node_listener, &node_events, nullptr);
        std::cout << std::format("object: id:{} type:{}/{}", id, type, version) << std::endl;
    }
}

static const struct pw_registry_events registry_events = {
    .version = PW_VERSION_REGISTRY_EVENTS,
    .global = registry_event_global,
    .global_remove = nullptr
};

int main(int argc, char *argv[])
{
    struct data data;
    struct pw_main_loop *loop = nullptr;
    struct pw_context *context = nullptr;
    struct pw_core *core = nullptr;
    struct pw_registry *registry = nullptr;
    struct spa_hook registry_listener;
    struct spa_hook node_listener;

    pw_init(&argc, &argv);

    loop = pw_main_loop_new(nullptr);
    context = pw_context_new(pw_main_loop_get_loop(loop), nullptr, 0);
    core = pw_context_connect(context, nullptr, 0);
    registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

    data.loop = loop;
    data.context = context;
    data.core = core;
    data.registry = registry;
    data.node_listener = node_listener;

    spa_zero(registry_listener);
    pw_registry_add_listener(registry, &registry_listener, &registry_events, &data);
    pw_main_loop_run(loop);

    pw_proxy_destroy((struct pw_proxy*)registry);
    pw_core_disconnect(core);
    pw_context_destroy(context);
    pw_main_loop_destroy(loop);

    return 0;
}
