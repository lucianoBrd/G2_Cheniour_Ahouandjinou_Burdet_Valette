<?php

declare(strict_types=1);

namespace DoctrineMigrations;

use Doctrine\DBAL\Schema\Schema;
use Doctrine\Migrations\AbstractMigration;

/**
 * Auto-generated Migration: Please modify to your needs!
 */
final class Version20211202142313 extends AbstractMigration
{
    public function getDescription(): string
    {
        return '';
    }

    public function up(Schema $schema): void
    {
        // this up() migration is auto-generated, please modify it to your needs
        $this->addSql('CREATE TABLE action (id INT AUTO_INCREMENT NOT NULL, value VARCHAR(255) NOT NULL, datetime DATETIME NOT NULL, state TINYINT(1) NOT NULL, PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE element (id INT AUTO_INCREMENT NOT NULL, room_id INT NOT NULL, type_id INT NOT NULL, label VARCHAR(255) NOT NULL, battery INT DEFAULT NULL, INDEX IDX_41405E3954177093 (room_id), INDEX IDX_41405E39C54C8C93 (type_id), PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE element_action (element_id INT NOT NULL, action_id INT NOT NULL, INDEX IDX_CEDB85061F1F2A24 (element_id), INDEX IDX_CEDB85069D32F035 (action_id), PRIMARY KEY(element_id, action_id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE home (id INT AUTO_INCREMENT NOT NULL, label VARCHAR(255) NOT NULL, PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE room (id INT AUTO_INCREMENT NOT NULL, home_id INT NOT NULL, label VARCHAR(255) NOT NULL, INDEX IDX_729F519B28CDC89C (home_id), PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE type (id INT AUTO_INCREMENT NOT NULL, label VARCHAR(255) NOT NULL, PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('CREATE TABLE value (id INT AUTO_INCREMENT NOT NULL, element_id INT NOT NULL, value VARCHAR(255) NOT NULL, datetime DATETIME NOT NULL, INDEX IDX_1D7758341F1F2A24 (element_id), PRIMARY KEY(id)) DEFAULT CHARACTER SET utf8mb4 COLLATE `utf8mb4_unicode_ci` ENGINE = InnoDB');
        $this->addSql('ALTER TABLE element ADD CONSTRAINT FK_41405E3954177093 FOREIGN KEY (room_id) REFERENCES room (id)');
        $this->addSql('ALTER TABLE element ADD CONSTRAINT FK_41405E39C54C8C93 FOREIGN KEY (type_id) REFERENCES type (id)');
        $this->addSql('ALTER TABLE element_action ADD CONSTRAINT FK_CEDB85061F1F2A24 FOREIGN KEY (element_id) REFERENCES element (id) ON DELETE CASCADE');
        $this->addSql('ALTER TABLE element_action ADD CONSTRAINT FK_CEDB85069D32F035 FOREIGN KEY (action_id) REFERENCES action (id) ON DELETE CASCADE');
        $this->addSql('ALTER TABLE room ADD CONSTRAINT FK_729F519B28CDC89C FOREIGN KEY (home_id) REFERENCES home (id)');
        $this->addSql('ALTER TABLE value ADD CONSTRAINT FK_1D7758341F1F2A24 FOREIGN KEY (element_id) REFERENCES element (id)');
    }

    public function down(Schema $schema): void
    {
        // this down() migration is auto-generated, please modify it to your needs
        $this->addSql('ALTER TABLE element_action DROP FOREIGN KEY FK_CEDB85069D32F035');
        $this->addSql('ALTER TABLE element_action DROP FOREIGN KEY FK_CEDB85061F1F2A24');
        $this->addSql('ALTER TABLE value DROP FOREIGN KEY FK_1D7758341F1F2A24');
        $this->addSql('ALTER TABLE room DROP FOREIGN KEY FK_729F519B28CDC89C');
        $this->addSql('ALTER TABLE element DROP FOREIGN KEY FK_41405E3954177093');
        $this->addSql('ALTER TABLE element DROP FOREIGN KEY FK_41405E39C54C8C93');
        $this->addSql('DROP TABLE action');
        $this->addSql('DROP TABLE element');
        $this->addSql('DROP TABLE element_action');
        $this->addSql('DROP TABLE home');
        $this->addSql('DROP TABLE room');
        $this->addSql('DROP TABLE type');
        $this->addSql('DROP TABLE value');
    }
}
