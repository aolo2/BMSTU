package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;

public class GroupingComparator extends WritableComparator {
    public GroupingComparator() {
        super(CompositeKeyWritableComparable.class, true);
    }

    @Override
    public int compare(WritableComparable a, WritableComparable b) {
        CompositeKeyWritableComparable x = (CompositeKeyWritableComparable) a;
        CompositeKeyWritableComparable y = (CompositeKeyWritableComparable) b;

        return x.getAirportId() - y.getAirportId();
    }
}
