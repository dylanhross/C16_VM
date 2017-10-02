#!/usr/local/bin/python3

"""
    C16_assebler.py

    utility for assembling C16_VM code into binary format that can
    be directly executed by the VM.
"""


import struct
from os.path import splitext


class Instruction():
    """
Instruction
    description:
        datastructure for encoding C16_VM instructions
"""

    # size of instruction (in bytes)
    instr_size = 6
    # dictionary mapping opcode names to single byte representations
    opcodes = {
        # NO OPERATION
        "NOOP":b'\x00',
        # stack instructions
        "IPSH":b'\x01',
        "IPOP":b'\x02',
        "FPSH":b'\x03',
        "FPOP":b'\x04',
        # memory access instructions
        "IRDM":b'\x05',
        "IWTM":b'\x06',
        "FRDM":b'\x07',
        "FWTM":b'\x08',
        # jump instructions
        "JUMP":b'\x09',
        "IJEQ":b'\x0A',
        "IJGT":b'\x0B',
        "IJGE":b'\x0C',
        "IJLT":b'\x0D',
        "IJLE":b'\x0E',
        # binary arithmetic instructions
        "AND":b'\x0F',
        "OR":b'\x10',
        "XOR":b'\x11',
        "NOT":b'\x12',
        "RSFT":b'\x13',
        "LSFT":b'\x14',
        # integer arithmetic instructions
        "IADD":b'\x15',
        "ISUB":b'\x16',
        "IMUL":b'\x17',
        "IDIV":b'\x18',
        "IMOD":b'\x19',
        # floating point arithmetic instructions
        "FADD":b'\x1A',
        "FSUB":b'\x1B',
        "FMUL":b'\x1C',
        "FDIV":b'\x1D',
        # call/return instructions
        "CALL":b'\x1E',
        "RET":b'\x1F',
        # immediate value instructions
        "ISET":b'\x20',
        "FSET":b'\x21',
        # move instructions
        "IMOV":b'\x22',
        "FMOV":b'\x23',
        "IMEQ":b'\x24',
        "IMNE":b'\x25',
        "IMLT":b'\x26',
        "IMLE":b'\x27',
        "IMGT":b'\x28',
        "IMGE":b'\x29',
        # end of execution
        "HALT":b'\x2A'}
    # dictionary mapping the various instruction formats to opcodes
    instr_fmts = {
            # OPCODE
            "NOOP":["op"],
            "RET":["op"],
            "HALT":["op"],
            # OPCODE IMM_SHORT
            "JUMP":["op", "imm_short"],
            "CALL":["op", "imm_short"],
            # OPCODE REG
            "IPSH":["op", "reg"],
            "IPOP":["op", "reg"],
            "FPSH":["op", "reg"],
            "FPOP":["op", "reg"],
            "NOT":["op", "reg"],
            # OPCODE REG IMM_SHORT
            "IRDM":["op", "reg", "imm_short"],
            "IWTM":["op", "reg", "imm_short"],
            "FRDM":["op", "reg", "imm_short"],
            "FWTM":["op", "reg", "imm_short"],
            "RSFT":["op", "reg", "imm_short"],
            "LSFT":["op", "reg", "imm_short"],
            "ISET":["op", "reg", "imm_short"],
            # OPCODE REG IMM_FLOAT
            "FSET":["op", "reg", "imm_float"],
            # OPCODE REG REG
            "AND":["op", "reg", "reg"],
            "OR":["op", "reg", "reg"],
            "XOR":["op", "reg", "reg"],
            "IADD":["op", "reg", "reg"],
            "ISUB":["op", "reg", "reg"],
            "IMUL":["op", "reg", "reg"],
            "IDIV":["op", "reg", "reg"],
            "IMOD":["op", "reg", "reg"],
            "FADD":["op", "reg", "reg"],
            "FMUL":["op", "reg", "reg"],
            "FSUB":["op", "reg", "reg"],
            "FDIV":["op", "reg", "reg"],
            "IMOV":["op", "reg", "reg"],
            "FMOV":["op", "reg", "reg"],
            # OPCODE REG REG REG REG
            "IMEQ":["op", "reg", "reg", "reg", "reg"],
            "IMNE":["op", "reg", "reg", "reg", "reg"],
            "IMLT":["op", "reg", "reg", "reg", "reg"],
            "IMLE":["op", "reg", "reg", "reg", "reg"],
            "IMGT":["op", "reg", "reg", "reg", "reg"],
            "IMGE":["op", "reg", "reg", "reg", "reg"],
            # OPCODE REG REG IMM_SHORT
            "IJEQ":["op", "reg", "reg", "imm_short"],
            "IJGT":["op", "reg", "reg", "imm_short"],
            "IJGE":["op", "reg", "reg", "imm_short"],
            "IJLT":["op", "reg", "reg", "imm_short"],
            "IJLE":["op", "reg", "reg", "imm_short"]}
    # dictionary mapping register names to bytes
    reg_names = {
            # int registers
            "rpc":b'\x00',
            "rsp":b'\x01',
            "rbp":b'\x02',
            "ia0":b'\x03',
            "ia1":b'\x04',
            "ia2":b'\x05',
            "ia3":b'\x06',
            "irv":b'\x07',
            # float registers
            "fa0":b'\x00',
            "fa1":b'\x01',
            "fa2":b'\x02',
            "fa3":b'\x03',
            "frv":b'\x04'}
    # address (for assembling purposes)
    addr_ = None

    def __init__(self, line):
        """
Instruction.__init__
    description:
        parses an instruction from a string
    parameters:
        line (str) -- line contianing instruction
    returns:
        n/a
"""
        # split into tokens
        tokens = line.strip().split()
        # store the opcode byte
        if tokens[0] not in self.opcodes:
            raise ValueError("Instruction: __init__: instruction '" + tokens[0] + "' not recognized")
        # look up the instruction format
        instr_fmt = self.instr_fmts[tokens[0]]
        # parse the rest of the instruction
        self.encoded_ = bytearray(self.opcodes[tokens[0]])
        for i in range(1, len(instr_fmt)):
            if instr_fmt[i] == "reg":
                if tokens[i] not in self.reg_names:
                    raise ValueError("Instruction: __init__: register '" + tokens[i] +
                                     "' not recognized")
                self.encoded_ += bytearray(self.reg_names[tokens[i]])
            elif instr_fmt[i] == "imm_short":
                self.encoded_ += bytearray.fromhex(tokens[i].lstrip("0").lstrip("x"))
            elif instr_fmt[i] == "imm_float":
                self.encoded_ += struct.pack("f", float(tokens[i]))
        # pad the encoded instruction with 0x00 up to 6 bytes
        while len(self.encoded_) < self.instr_size:
            self.encoded_ += bytearray(b'\x00')


class Assembler():
    """
Assembler
    description:

"""

    # instruction size taken from Instruction class
    instr_size = Instruction.instr_size
    # always start with label '_main_'
    start_label = "_main_"
    # instructions that require addresses
    ireqaddrs = ["JUMP", "IJEQ", "IJGT", "IJGE", "IJLT", "IJLE", "CALL"]
    # instruction placeholder fill value
    dummy_fill = bytearray(b'\xAA\xBB\xCC\xDD\xEE\xFF')

    def __init__(self, source_file):
        """
Assembler.__init__
    description:
        initializes a new Assembler object using an input source file
    parameters:
        source_file (str) -- source file name
    returns:
        n/a
"""
        # store the source file name
        self.source_ = source_file
        # parse the source file
        self.parseSource()
        # build an encoded binary
        self.buildBinary()

    def parseSource(self):
        """
Assembler.parseSource
    description:
        parses the source file in self.source_ and fills the self.labels_ dict with instructions
    parameters:
        n/a
    returns:
        n/a
"""
        # parse the input file line by line
        self.labels_ = {}
        current_label = None
        with open(self.source_, "r") as src:
            for line in src.readlines():
                # skip comments and blank lines
                ls = line.strip()
                if ls and line[0] != "#":
                    # check for new labels
                    if ls[-1] == ":":
                        current_label = ls[:-1]
                        self.labels_[current_label] = []
                    # add the instruction to the current labels dict under the current label
                    else:
                        # check for any commands requiring address calculations
                        req_addr = False
                        for instr in self.ireqaddrs:
                            if instr in ls:
                                req_addr = True
                                # add the line as a string instead of as an Instruction
                                self.labels_[current_label].append(ls)
                        if not req_addr and ls[0][0] != "#":
                            # for all others add an Instruction object
                            self.labels_[current_label].append(Instruction(ls))

    def buildBinary(self):
        """
Assembler.buildBinary
    description:
        encodes the instructions and defines code positions of the labels and stores the
        encoded data in a bytearray self.encoded_
    parameters:
    returns:
        n/a
"""
        # executable address counter
        addr = 0
        # add instructions to the self.encoded_ bytearray, if there is a string, note the position
        # and add it into a temporary dict.
        self.encoded_ = bytearray()
        need_label_trans = {}
        # code addresses of labels, already know _main_ is at 0
        label_addr = {"_main_":"0x0000"}
        # start with the _main_ label, which is required
        for instr in self.labels_["_main_"]:
            # any strings need name resolution, deal with those later
            if type(instr) is str:
                # store the address
                need_label_trans[instr] = addr
                self.encoded_ += self.dummy_fill
            else:
                self.encoded_ += instr.encoded_
            addr += self.instr_size
        # now define other labels
        for label in self.labels_:
            if label != "_main_":
                label_addr[label] = hex(addr)
                for instr in self.labels_[label]:
                    # any strings need name resolution, deal with those later
                    if type(instr) is str:
                        # store the address
                        need_label_trans[instr] = addr
                        self.encoded_ += self.dummy_fill
                    else:
                        self.encoded_ += instr.encoded_
                    addr += self.instr_size
        # translate labels into addresses
        for instr in need_label_trans:
            label = instr.split()[-1]
            new_instr = Instruction(instr.replace(label, label_addr[label]))
            self.encoded_ = self.encoded_[:need_label_trans[instr]] + \
                            new_instr.encoded_ + \
                            self.encoded_[need_label_trans[instr] + self.instr_size:]
        # store the number of instructions in the binary
        # stored encoded as an unsigned short (2bytes)
        hx = hex(int(len(self.encoded_) / self.instr_size)).lstrip("0").lstrip("x")
        if len(hx) % 2:
            hx = "0" + hx
        self.n_instr_ = bytearray.fromhex(hx)
        # ensure the length is 2 bytes
        if len(self.n_instr_) < 2:
            self.n_instr_ += bytearray(b'\x00')
        # DONE

    def writeBinary(self, override_file_name=None):
        """
Assembler.writeBinary
    description:
        writes the assembled binary to file, name either generated from input file
        name or provided as an optional parameter
    parameters:
        [override_file_name (None or str)] -- if a string is provided it is used to
                                              name the binary file that is produced
                                              otherwise the file name is generated
                                              from the input file name
"""
        if override_file_name:
            self.binary_ = override_file_name
        else:
            self.binary_ = splitext(self.source_)[0] + ".b"
        # write the encoded data to file
        with open(self.binary_, "wb") as out:
            # first write any data that needs to be at the beginning of the file
            # currently, only the number of instructions to execute
            out.write(asm.n_instr_)
            # write the encoded instructions
            out.write(self.encoded_)



# testing purposes
asm = Assembler("f_isq.C16.a")
for i in range(0, len(asm.encoded_), 6):
    print(asm.encoded_[i:i + 6].hex())
asm.writeBinary()



