#include <pipewire/pipewire.h>

#include <iostream>
#include <format>
#include <string>

static void registry_event_global(void *data, uint32_t id,
                uint32_t permissions, const char *type, uint32_t version,
                const struct spa_dict *props)
{
    // printf("object: id:%u type:%s/%d\n", id, type, version);
    (void)data;
    (void)permissions;
    (void)props;
    std::cout << std::format("object: id:{} type:{}/{}", id, type, version) << std::endl;
}

static const struct pw_registry_events registry_events = {
    .version = PW_VERSION_REGISTRY_EVENTS,
    .global = registry_event_global,
    .global_remove = nullptr
};
 

int main(int argc, char *argv[])
{
    struct pw_main_loop *loop = nullptr;
    struct pw_context *context = nullptr;
    struct pw_core *core = nullptr;
    struct pw_registry *registry = nullptr;
    struct spa_hook registry_listener;
    (void)registry;
    (void)registry_listener;

    pw_init(&argc, &argv);

    loop = pw_main_loop_new(nullptr);
    context = pw_context_new(pw_main_loop_get_loop(loop), nullptr, 0);

    core = pw_context_connect(context, nullptr, 0);

    registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

    spa_zero(registry_listener);
    pw_registry_add_listener(registry, &registry_listener, &registry_events, nullptr);

    pw_main_loop_run(loop);

    pw_proxy_destroy((struct pw_proxy*)registry);
    pw_core_disconnect(core);
    pw_context_destroy(context);
    pw_main_loop_destroy(loop);

    return 0;
}
