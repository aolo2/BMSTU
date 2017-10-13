package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.WritableComparable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class CompositeKeyWritableComparable implements WritableComparable<CompositeKeyWritableComparable> {
    private int airportId;
    private int dataFlag;

    CompositeKeyWritableComparable() { }

    public CompositeKeyWritableComparable(int airportId, int dataFlag) {
        this.airportId = airportId;
        this.dataFlag = dataFlag;
    }

    @Override
    public int hashCode() {
        return (String.valueOf(airportId) + String.valueOf(dataFlag)).hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        CompositeKeyWritableComparable other = (CompositeKeyWritableComparable) o;
        return airportId == other.airportId && dataFlag == other.dataFlag;
    }

    public int compareTo(CompositeKeyWritableComparable other) {
        int more = airportId - other.airportId;

        if (more == 0) {
            return dataFlag - other.dataFlag;
        }

        return more;
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(airportId);
        dataOutput.writeInt(dataFlag);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        airportId = dataInput.readInt();
        dataFlag = dataInput.readInt();
    }

    public int getAirportId() {
        return airportId;
    }
}
