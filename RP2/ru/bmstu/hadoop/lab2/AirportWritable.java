package ru.bmstu.hadoop.lab2;

import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.io.Writable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Optional;

public class AirportWritable implements Writable {
    private static final int CODE_POS = 0;
    private static final int DESC_POS = 1;

    private int code;
    private String fullName;

    private AirportWritable(int code, String fullName) {
        this.code = code;
        this.fullName = fullName;
    }

    public static Optional<AirportWritable> read(String entry) {
        String[] fields = entry.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");

        AirportWritable.cutQuotes(fields);

        if (fields[CODE_POS].isEmpty() || !StringUtils.isNumeric(fields[CODE_POS])) {
            return Optional.empty();
        }

        int code = Integer.parseInt(fields[CODE_POS]);
        String fullName = fields[DESC_POS];

        return Optional.of(new AirportWritable(code, fullName));
    }

    private static void cutQuotes(String[] words) {
        for (int i = 0; i < words.length; i++) {
            words[i] = words[i].replaceAll("\"", "");
        }
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(code);
        dataOutput.writeUTF(fullName);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        code = dataInput.readInt();
        fullName = dataInput.readUTF();
    }

    public String getFullName() {
        return fullName;
    }

    public int getCode() {
        return code;
    }
}
