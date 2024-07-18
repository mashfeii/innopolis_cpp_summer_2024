#!/usr/bin/env python3

from random import randint
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


def test_dumps_with_single_dict():
    test_dict = {}
    for i in range(1000):
        test_dict[fake.pystr(min_chars=1, max_chars=20000)] = fake.random_element(
            elements=(fake.random_int(min=0, max=250000),
                      fake.pystr(min_chars=1, max_chars=250000)))
    assert (cjson.dumps(test_dict)) == json.dumps(test_dict)


def test_dumps_with_several_dicts():
    for i in range(1000):
        test_dict = fake.pydict(nb_elements=100,
                                variable_nb_elements=False,
                                value_types=[int, str])
        assert (cjson.dumps(test_dict)) == json.dumps(test_dict)


def test_loads():
    for i in range(1000):
        structure = fake.pydict(nb_elements=randint(
            4, 50), value_types=[fake.types, fake.types])
        json_str = fake.json(data_columns=structure, num_rows=1)
        assert (cjson.loads(json_str)) == ujson.loads(json_str)
        assert (cjson.loads(json_str)) == json.loads(json_str)
