package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class JoinPartitioner extends Partitioner<CompositeKeyWritableComparable, Text> {
    @Override
    public int getPartition(CompositeKeyWritableComparable key, Text value, int n) {
        return key.getAirportId() % n;
    }
}
