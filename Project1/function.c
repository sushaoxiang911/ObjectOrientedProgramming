void unrecognized_command();

void print_record_title(struct Ordered_container* title_library);

void print_record_id(struct Ordered_container* id_library);

void print_collection(struct Ordered_container* catalog);

void print_library(struct Ordered_container* title_library);

void print_catalog(struct Ordered_container* catalog);

void print_allocation(struct Ordered_container* title_library, struct Ordered_container* id_library);

void add_record(struct Ordered_container* title_library, struct Ordered_container* id_library);

void add_collection(struct Ordered_container* catalog);

void add_record_to_collection(struct Ordered_container* catalog, struct Ordered_container* id_library);

void modify_record_rating(struct Ordered_container* id_library);

void delete_record_from_library(struct Ordered_container* title_library, struct Ordered_container* id_library, struct Ordered_container* catalog);

void delete_collection_from_catalog(struct Ordered_container* catalog);


void delete_record_from_collection(struct Ordered_container* catalog, struct Ordered_container* id_library);

void save_all(struct Ordered_container* title_library, struct Ordered_container* catalog);

void restore_all(struct Ordered_container* title_library, struct Ordered_container* id_library, struct Ordered_container* catalog);

void quit(struct Ordered_container* title_library, struct Ordered_container* id_library, struct Ordered_container* catalog);
