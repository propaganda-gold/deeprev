#ifndef UTIL_MAIN_INCL
#define UTIL_MAIN_INCL 
int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  auto status = Main();
  Output_value(status.status());
  if (status.ok()) {
    return 0;
  }

  Output_value(status.error().message());
  Output_value(status.error().StackTrace());
  return 1;
}
#endif /* UTIL_MAIN_INCL */
