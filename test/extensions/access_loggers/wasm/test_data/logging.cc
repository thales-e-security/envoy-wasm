// NOLINT(namespace-envoy)
#include <string>
#include <unordered_map>

#include "proxy_wasm_intrinsics.h"

class ExampleContext : public Context {
public:
  explicit ExampleContext(uint32_t id) : Context(id) {}

  FilterHeadersStatus onRequestHeaders() override;
  FilterDataStatus onRequestBody(size_t body_buffer_length, bool end_of_stream) override;
  void onLog() override;
  void onDone() override;
};

std::unique_ptr<Context> NewContext(uint32_t id) {
  return std::unique_ptr<Context>(new ExampleContext(id));
}

FilterHeadersStatus ExampleContext::onRequestHeaders() {
  logDebug(std::string("onRequestHeaders ") + std::to_string(id()));
  auto path = getRequestHeader(":path");
  logInfo(std::string("header path ") + std::string(path->view()));
  addRequestHeader("newheader", "newheadervalue");
  replaceRequestHeader("server", "envoy-wasm");
  return FilterHeadersStatus::Continue;
}

FilterDataStatus ExampleContext::onRequestBody(size_t body_buffer_length, bool end_of_stream) {
  auto body = getRequestBodyBufferBytes(0, body_buffer_length);
  logError(std::string("onRequestBody ") + std::string(body->view()));
  return FilterDataStatus::Continue;
}

void ExampleContext::onLog() {
  auto path = getRequestHeader(":path");
  logWarn("onLog " + std::to_string(id()) + " " + std::string(path->view()));
}

void ExampleContext::onDone() { logWarn("onDone " + std::to_string(id())); }
