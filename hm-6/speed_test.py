#!/usr/bin/env python3

from time import time_ns
import cjson
import ujson
import json
from faker import Faker
from faker.providers import DynamicProvider
fake = Faker()

types = DynamicProvider(
    provider_name="types",
    elements=["pyint", "pystr"],
)
fake.add_provider(types)

structure = fake.pydict(nb_elements=5000, value_types=[
                        fake.types, fake.types])
json_str = fake.json(data_columns=structure, num_rows=1)

test_dict = {}
for i in range(5000):
    test_dict[fake.pystr(min_chars=1, max_chars=20000)] = fake.random_element(
        elements=(fake.random_int(min=0, max=250000),
                  fake.pystr(min_chars=1, max_chars=250000)))


def time_dumps(function, obj):
    start_time = time_ns()
    function(obj)
    end_time = time_ns()
    return (end_time - start_time) / 10 ** 9


def main():
    print("==== default json.dumps: ", end='')
    print(f"{time_dumps(json.dumps, test_dict)} s")
    print("==== pip ujson.dumps: ", end='')
    print(f"{time_dumps(ujson.dumps, test_dict)} s")
    print("==== module cjson.dumps: ", end='')
    print(f"{time_dumps(cjson.dumps, test_dict)} s")

    print("------------------------------------------------------------")

    print("==== default json.loads: ", end='')
    print(f"{time_dumps(json.loads, json_str)} s")
    print("==== pip ujson.loads: ", end='')
    print(f"{time_dumps(ujson.loads, json_str)} s")
    print("==== module cjson.loads: ", end='')
    print(f"{time_dumps(cjson.loads, json_str)} s")


if __name__ == "__main__":
    main()
