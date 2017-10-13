package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;
import java.util.Optional;

public class AirportJoinMapper extends Mapper<LongWritable, Text, CompositeKeyWritableComparable, Text> {
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        Optional<AirportWritable> opt = AirportWritable.read(value.toString());

        if (opt.isPresent()) {
            AirportWritable airport = opt.get();
            context.write(
                    new CompositeKeyWritableComparable(airport.getCode(), 0),
                    new Text(airport.getFullName()));
        }
    }
}
