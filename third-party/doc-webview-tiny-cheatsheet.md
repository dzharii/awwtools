
// navigate to URL
void navigate(const std::string &url)

// Add javascript oninit
void init(const std::string &js)


// eval javascript
void eval(const std::string &js)

// set html
void set_html(const std::string &html)

// Synchronous bind
void bind(const std::string &name, sync_binding_t fn)

// Asynchronous bind
void bind(const std::string &name, binding_t f, void *arg)

void unbind(const std::string &name)

void resolve(const std::string &seq, int status, const std::string &result)
