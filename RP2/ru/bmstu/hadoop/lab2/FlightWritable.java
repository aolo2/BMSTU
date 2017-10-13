package ru.bmstu.hadoop.lab2;

import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.io.Writable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Optional;

public class FlightWritable implements Writable {
    private static final int DEST_AIRPORT_ID_POS = 14;
    private static final int CANCELLED_POS = 19;
    private static final int DELAY_POS = 18;
    private static final int NUM_COLUMNS = 23;

    private int destAirportId;
    private float delayMinutes;

    private FlightWritable(int destAirportId, float delayMinutes) {
        this.destAirportId = destAirportId;
        this.delayMinutes = delayMinutes;
    }

    public static Optional<FlightWritable> read(String entry) {
        String[] fields = entry.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");

        FlightWritable.cutQuotes(fields);

        if (fields.length != NUM_COLUMNS
                || fields[DEST_AIRPORT_ID_POS].isEmpty()
                || fields[DELAY_POS].isEmpty()
                || fields[CANCELLED_POS].isEmpty()
                || !StringUtils.isNumeric(fields[DEST_AIRPORT_ID_POS].replace(".", ""))
                || !StringUtils.isNumeric(fields[DELAY_POS].replace(".", ""))
                || !StringUtils.isNumeric(fields[CANCELLED_POS].replace(".", ""))) {

            return Optional.empty();
        }


        int destAirportId = Integer.parseInt(fields[DEST_AIRPORT_ID_POS]);
        boolean canceled = fields[CANCELLED_POS].equals("1.00");
        float delayMinutes = Float.parseFloat(fields[DELAY_POS]);

        if (delayMinutes == 0.0f || canceled) {
            return Optional.empty();
        }

        return Optional.of(new FlightWritable(destAirportId, delayMinutes));
    }

    private static void cutQuotes(String[] words) {
        for (int i = 0; i < words.length; i++) {
            words[i] = words[i].replaceAll("\"", "");
        }
    }

    public int getDestAirportId() {
        return destAirportId;
    }

    public float getDelayMinutes() {
        return delayMinutes;
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(destAirportId);
        dataOutput.writeFloat(delayMinutes);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        destAirportId = dataInput.readInt();
        delayMinutes = dataInput.readFloat();
    }
}
