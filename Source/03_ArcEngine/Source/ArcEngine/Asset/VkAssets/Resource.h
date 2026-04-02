#pragma once

#include <string>

class Resource {
private:
    std::string resourceId;     // Unique identifier for this resource within the system
    bool loaded = false;        // Loading state flag for resource lifecycle management

public:
    explicit Resource(const std::string& id) : resourceId(id) {}
    virtual ~Resource() = default;

    // Core resource identity and state access methods
    const std::string& GetId() const { return resourceId; }
    bool IsLoaded() const { return loaded; }

    // Virtual interface for resource-specific loading and unloading behavior
    virtual bool Load() {
        loaded = doLoad();
        return loaded;
    }

    virtual void Unload() {
        doUnload();
        loaded = false;
    }

protected:
    virtual bool doLoad() = 0;
    virtual bool doUnload() = 0;
};