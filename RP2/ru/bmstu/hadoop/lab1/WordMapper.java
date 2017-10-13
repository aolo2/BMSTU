package ru.bmstu.hadoop.lab1;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class WordMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        String[] words = value.toString().toLowerCase().split("[^a-zA-Zа-яА-Я]");

        for (String word : words) {
            context.write(new Text(word), new IntWritable(1));
        }
    }
}
