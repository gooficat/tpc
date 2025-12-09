instructions : list[tuple[str, int]] = [
    ("add", 0x00),
    ("push", 0x06),
    ("pop", 0x07),
    ("mov", 0x88),
    ("retn", 0xC3),
    ("retf", 0xCB)
]

registers : list[tuple[str, int, int]] = [
    ("ax", 0x00, 2), # byteness
    ("cx", 0x01, 2),
    ("dx", 0x02, 2),
    ("bx", 0x03, 2),

    ("sp", 0x04, 2),
    ("bp", 0x05, 2),
    ("si", 0x06, 2),
    ("di", 0x07, 2),


    ("al", 0x00, 1),
    ("cl", 0x01, 1),
    ("dl", 0x02, 1),
    ("bl", 0x03, 1),

    ("ah", 0x04, 1),
    ("ch", 0x05, 1),
    ("dh", 0x06, 1),
    ("bh", 0x07, 1),
]