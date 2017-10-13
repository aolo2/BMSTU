package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;
import java.util.Optional;

public class FlightJoinMapper extends Mapper<LongWritable, Text, CompositeKeyWritableComparable, Text> {
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        Optional<FlightWritable> opt = FlightWritable.read(value.toString());

        if (opt.isPresent()) {
            FlightWritable flight = opt.get();
            context.write(
                    new CompositeKeyWritableComparable(flight.getDestAirportId(), 1),
                    new Text(String.valueOf(flight.getDelayMinutes())));
        }
    }
}
